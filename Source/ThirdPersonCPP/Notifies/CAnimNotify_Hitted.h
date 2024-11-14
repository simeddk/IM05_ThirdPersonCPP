#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_Hitted.generated.h"

//Hack 08. 맞는 몽타쥬 재생 후 Idle, bCanMove 등 풀어주는 노티파이
UCLASS()
class THIRDPERSONCPP_API UCAnimNotify_Hitted : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
