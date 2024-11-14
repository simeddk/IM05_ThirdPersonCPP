#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDPERSONCPP_API UCAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCAttributeComponent();

protected:
	virtual void BeginPlay() override;

public:
	FORCEINLINE float GetMaxHealth() { return MaxHealth; }
	FORCEINLINE float GetCurrentHealth() { return CurrentHealth; }

	FORCEINLINE float GetSnekSpeed() { return SneakSpeed; }
	FORCEINLINE float GetWalkSpeed() { return WalkSpeed; }
	FORCEINLINE float GetSprintSpeed() { return SprintSpeed; }
	FORCEINLINE bool IsCanMove() { return bCanMove; }

	void SetMove();
	void SetStop();

protected:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Health")
	float MaxHealth;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Speed")
	float SneakSpeed;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Speed")
	float WalkSpeed;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Speed")
	float SprintSpeed;

private:
	float CurrentHealth;
	bool bCanMove;
};
