#include "CDoAction_Warp.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CAttributeComponent.h"
#include "CAttachment.h"

void ACDoAction_Warp::BeginPlay()
{
	Super::BeginPlay();

	for (const auto& Child : OwnerCharacter->Children)
	{
		if (Child->IsA<ACAttachment>() && Child->GetActorLabel().Contains("Warp"))
		{
			PerviewMeshComp = CHelpers::GetComponent<USkeletalMeshComponent>(Child);
			break;
		}
	}
}

void ACDoAction_Warp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PerviewMeshComp->SetVisibility(false);

	CheckFalse(*bEquipped);

	FVector CursorLocationToWorld;
	if (GetCursorLocation(CursorLocationToWorld))
	{
		PerviewMeshComp->SetVisibility(true);
		PerviewMeshComp->SetWorldLocation(CursorLocationToWorld);
	}
}

void ACDoAction_Warp::PrimaryAction()
{
	Super::PrimaryAction();

	CheckFalse(Datas.Num() > 0);
	CheckFalse(StateComp->IsIdleMode());

	CheckFalse(GetCursorLocation(LocationToWarp));

	StateComp->SetActionMode();
	OwnerCharacter->PlayAnimMontage(Datas[0].Montage, Datas[0].PlayRate, Datas[0].StartSection);
	Datas[0].bCanMove ? AttributeComp->SetMove() : AttributeComp->SetStop();

	SetPerviewMeshColor(FLinearColor::Red);
}

void ACDoAction_Warp::Begin_PrimaryAction()
{
	Super::Begin_PrimaryAction();

	UParticleSystem* Effect = Datas[0].Effect;

	if (Effect)
	{
		UGameplayStatics::SpawnEmitterAttached
		(
			Effect,
			OwnerCharacter->GetMesh(),
			"Hand_ThrowItem",
			Datas[0].EffectTransfrom.GetLocation(),
			FRotator(Datas[0].EffectTransfrom.GetRotation()),
			Datas[0].EffectTransfrom.GetScale3D()
		);
	}
}

void ACDoAction_Warp::End_PrimaryAction()
{
	Super::End_PrimaryAction();

	LocationToWarp.Z += OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	OwnerCharacter->SetActorLocation(LocationToWarp);

	StateComp->SetIdleMode();
	AttributeComp->SetMove();

	SetPerviewMeshColor(FLinearColor(0, 1, 1));
}

bool ACDoAction_Warp::GetCursorLocation(FVector& OutLocation)
{
	APlayerController* PC = OwnerCharacter->GetController<APlayerController>();

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(ObjectTypeQuery1);

	FHitResult Hit;
	if (PC->GetHitResultUnderCursorForObjects(ObjectTypes, true, Hit))
	{
		OutLocation = Hit.Location;
		return true;
	}


	return false;
}

void ACDoAction_Warp::SetPerviewMeshColor(FLinearColor InColor)
{
	FVector Emissive = FVector(InColor.R, InColor.G, InColor.B);
	PerviewMeshComp->SetVectorParameterValueOnMaterials("Emissive", Emissive);
}
