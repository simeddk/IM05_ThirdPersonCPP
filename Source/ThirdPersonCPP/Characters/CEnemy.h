#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/CCharacterInterface.h"
#include "Components/CStateComponent.h"
#include "CEnemy.generated.h"

class UCAttributeComponent;
class UCStateComponent;
class UCMontagesComponent;
class UCActionComponent;
class UMaterialInstanceDynamic;
class UWidgetComponent;

UCLASS()
class THIRDPERSONCPP_API ACEnemy : public ACharacter, public ICCharacterInterface
{
	GENERATED_BODY()

public:
	ACEnemy();

protected:
	virtual void BeginPlay() override;

	//Hack 01. 적에게 대미지가 들어가도록 이 함수를 작성했었음(겸사겸사 AttributeComp의 Increase, DecreaseHP 함수도 작성함)
public:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

public:
	virtual void SetBodyColor(FLinearColor InColor) override;

	//Hack 05. 상태 변경에 대한 콜백 함수 작성함(겸사겸사 MontagesComp의 PlayHitted 함수도 작성함)
private:
	UFUNCTION()
	void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

	void Hitted();
	void Dead();

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCAttributeComponent* AttributeComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCStateComponent* StateComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCMontagesComponent* MontagesComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UCActionComponent* ActionComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UWidgetComponent* NameWidgetComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UWidgetComponent* HealthWidgetComp;

private:
	UPROPERTY(EditAnywhere, Category = "Hitted")
	float LaunchValue;

private:
	AController* DamageInstigator;
	float DamageValue;

	UMaterialInstanceDynamic* BodyMaterial;
	UMaterialInstanceDynamic* LogoMaterial;
};
