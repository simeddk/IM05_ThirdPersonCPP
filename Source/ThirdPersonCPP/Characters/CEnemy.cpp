#include "CEnemy.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CAttributeComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CActionComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UI/CNameWidget.h"
#include "UI/CHealthWidget.h"
#include "Actions/CActionData.h"

ACEnemy::ACEnemy()
{
	//Mesh Comp
	USkeletalMesh* MeshAsset;
	CHelpers::GetAsset(&MeshAsset, "/Game/Character/Mesh/SK_Mannequin");
	GetMesh()->SetSkeletalMesh(MeshAsset);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -88));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	TSubclassOf<UAnimInstance> AnimClass;
	CHelpers::GetClass(&AnimClass, "/Game/Enemies/ABP_CEnemy");
	GetMesh()->SetAnimInstanceClass(AnimClass);

	//Action Comp
	CHelpers::CreateActorComponent(this, &ActionComp, "ActionComp");

	//Montages Comp
	CHelpers::CreateActorComponent(this, &MontagesComp, "MontagesComp");

	//AttributeComp
	CHelpers::CreateActorComponent(this, &AttributeComp, "AttributeComp");

	//State Comp
	CHelpers::CreateActorComponent(this, &StateComp, "StateComp");

	//Widget Comp
	CHelpers::CreateSceneComponent(this, &NameWidgetComp, "NameWidgetComp", GetMesh());
	CHelpers::CreateSceneComponent(this, &HealthWidgetComp, "HealthWidgetComp", GetMesh());
	
	// -> NameWidget
	TSubclassOf<UCNameWidget> NameWidgetClass;
	CHelpers::GetClass(&NameWidgetClass, "/Game/UI/WB_Name");
	NameWidgetComp->SetWidgetClass(NameWidgetClass);
	NameWidgetComp->SetRelativeLocation(FVector(0, 0, 240));
	NameWidgetComp->SetDrawSize(FVector2D(240, 30));
	NameWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);

	// -> HealthWidget
	TSubclassOf<UCHealthWidget> HealthWidgetClass;
	CHelpers::GetClass(&HealthWidgetClass, "/Game/UI/WB_Health");
	HealthWidgetComp->SetWidgetClass(HealthWidgetClass);
	HealthWidgetComp->SetRelativeLocation(FVector(0, 0, 190));
	HealthWidgetComp->SetDrawSize(FVector2D(120, 20));
	HealthWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);

	//Movement Comp
	GetCharacterMovement()->MaxWalkSpeed = AttributeComp->GetSprintSpeed();
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	//Property Settings
	LaunchValue = 30.f;
}

void ACEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	//Set Dynamic Material
	BodyMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), nullptr);
	LogoMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(1), nullptr);

	GetMesh()->SetMaterial(0, BodyMaterial);
	GetMesh()->SetMaterial(1, LogoMaterial);

	//On StateType Changed
	StateComp->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);
	ActionComp->SetUnaremdMode();

	//Widget Settings
	NameWidgetComp->InitWidget();
	UCNameWidget* NameWidgetObject = Cast<UCNameWidget>(NameWidgetComp->GetUserWidgetObject());
	if (NameWidgetObject)
	{
		NameWidgetObject->SetNameText(GetController()->GetName(), GetName());
	}

	HealthWidgetComp->InitWidget();
	UCHealthWidget* HealthWidgetObject = Cast<UCHealthWidget>(HealthWidgetComp->GetUserWidgetObject());
	if (HealthWidgetObject)
	{
		HealthWidgetObject->Update(AttributeComp->GetCurrentHealth(), AttributeComp->GetMaxHealth());
	}
}

float ACEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	DamageInstigator = EventInstigator;
	DamageValue = ActualDamage;

	AttributeComp->DecreaseHealth(Damage);

	if (AttributeComp->GetCurrentHealth() <= 0.f)
	{
		StateComp->SetDeadMode();
		return ActualDamage;
	}

	StateComp->SetHittedMode();

	return ActualDamage;
}

void ACEnemy::SetBodyColor(FLinearColor InColor)
{
	CheckTrue(StateComp->IsDeadMode());

	if (StateComp->IsHittedMode())
	{
		LogoMaterial->SetScalarParameterValue("bHitted", 1.f);
		LogoMaterial->SetVectorParameterValue("BodyColor", InColor);

		return;
	}

	BodyMaterial->SetVectorParameterValue("BodyColor", InColor);
	LogoMaterial->SetVectorParameterValue("BodyColor", InColor);
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Hitted:
		{
			Hitted();
		}
		break;

		case EStateType::Dead:
		{
			Dead();
		}
		break;
	}
}

void ACEnemy::Hitted()
{
	//Update Health Widget
	UCHealthWidget* HealthWidgetObject = Cast<UCHealthWidget>(HealthWidgetComp->GetUserWidgetObject());
	if (HealthWidgetObject)
	{
		HealthWidgetObject->Update(AttributeComp->GetCurrentHealth(), AttributeComp->GetMaxHealth());
	}

	//Play Hitted Motnages
	MontagesComp->PlayHitted();

	//Hit Back
	FVector Start = DamageInstigator->GetPawn()->GetActorLocation();
	FVector Target = GetActorLocation();
	FVector LaunchDirection = (Target - Start).GetSafeNormal();
	LaunchCharacter(LaunchDirection * DamageValue * LaunchValue, true, false);

	//Change Logo Color
	SetBodyColor(FLinearColor::Red * 1000.f);
	UKismetSystemLibrary::K2_SetTimer(this, "RestoreBodyColor", 0.5f, false);
}

void ACEnemy::Dead()
{
	FString Message = GetName();
	Message.Append(" is dead.");
	
	CLog::Print(Message, -1, 2.f, FColor::Red);
}

void ACEnemy::RestoreBodyColor()
{
	LogoMaterial->SetScalarParameterValue("bHitted", 0.f);

	if (ActionComp->GetCurrentDataAsset())
	{
		FLinearColor EquipmentColor = ActionComp->GetCurrentDataAsset()->EquipmentColor;
		LogoMaterial->SetVectorParameterValue("BodyColor", EquipmentColor);
	}
}
