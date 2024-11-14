#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimNotifyState_Collision.generated.h"

//Hack 03. 충돌체 켜고 끄는 노티파이스테이트 작성함 -> Sword Attack 1 Montage에 적용했음
UCLASS()
class THIRDPERSONCPP_API UCAnimNotifyState_Collision : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	FString GetNotifyName_Implementation() const override;
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
