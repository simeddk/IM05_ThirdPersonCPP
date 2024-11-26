#include "CActionComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Actions/CActionData.h"
#include "Actions/CEquipment.h"
#include "Actions/CDoAction.h"

UCActionComponent::UCActionComponent()
{

}


void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());

	for (int32 i = 0; i < (int32)(int32)EActionType::Max; i++)
	{
		if (DataAssets[i] && OwnerCharacter)
		{
			DataAssets[i]->BeginPlay(OwnerCharacter);
		}
	}
}

void UCActionComponent::PrimaryAction()
{
	CheckTrue(IsUnarmedMode());

	if (DataAssets[(int32)Type] && DataAssets[(int32)Type]->GetDoAction())
	{
		DataAssets[(int32)Type]->GetDoAction()->PrimaryAction();
	}
}

void UCActionComponent::Begin_SecondaryAction()
{
	CheckTrue(IsUnarmedMode());

	if (DataAssets[(int32)Type] && DataAssets[(int32)Type]->GetDoAction())
	{
		DataAssets[(int32)Type]->GetDoAction()->Begin_SecondaryAction();
	}
}

void UCActionComponent::End_SecondaryAction()
{
	CheckTrue(IsUnarmedMode());

	if (DataAssets[(int32)Type] && DataAssets[(int32)Type]->GetDoAction())
	{
		DataAssets[(int32)Type]->GetDoAction()->End_SecondaryAction();
	}
}

void UCActionComponent::SetUnaremdMode()
{
	if (DataAssets[(int32)Type] && DataAssets[(int32)Type]->GetEquipment())
	{
		DataAssets[(int32)Type]->GetEquipment()->Unequip();
	}

	if (DataAssets[(int32)EActionType::Unarmed] && DataAssets[(int32)EActionType::Unarmed]->GetEquipment())
	{
		DataAssets[(int32)EActionType::Unarmed]->GetEquipment()->Equip();
	}

	ChangeType(EActionType::Unarmed);
}

void UCActionComponent::SetFistMode()
{
	SetMode(EActionType::Fist);
}

void UCActionComponent::SetOneHandMode()
{
	SetMode(EActionType::OneHand);
}

void UCActionComponent::SetTwoHandMode()
{
	SetMode(EActionType::TwoHand);
}

void UCActionComponent::SetMagicBallMode()
{
	SetMode(EActionType::MagicBall);
}

void UCActionComponent::SetWarpMode()
{
	SetMode(EActionType::Warp);
}

void UCActionComponent::SetWhirlWindMode()
{
	SetMode(EActionType::WhirlWind);
}

void UCActionComponent::SetMode(EActionType InNewType)
{
	if (Type == InNewType)
	{
		SetUnaremdMode();
		return;
	}
	else if (!IsUnarmedMode())
	{
		if (DataAssets[(int32)Type] && DataAssets[(int32)Type]->GetEquipment())
		{
			DataAssets[(int32)Type]->GetEquipment()->Unequip();
		}
	}

	if (DataAssets[(int32)InNewType] && DataAssets[(int32)InNewType]->GetEquipment())
	{
		DataAssets[(int32)InNewType]->GetEquipment()->Equip();
	}

	ChangeType(InNewType);
}

void UCActionComponent::ChangeType(EActionType InNewType)
{
	EActionType PrevType = Type;
	Type = InNewType;
 
	OnActionTypeChanged.Broadcast(PrevType, Type);
}
