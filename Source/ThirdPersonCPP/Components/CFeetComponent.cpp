#include "CFeetComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

UCFeetComponent::UCFeetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	LeftSocket = "Foot_L";
	RightSocket = "Foot_R";

	Additional = 55.f;
	FootHeight = 5.f;

	DrawDebugType = EDrawDebugTrace::ForOneFrame;
}


void UCFeetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}


void UCFeetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float LeftDistance;
	Trace(LeftSocket, LeftDistance);

	float RightDistance;
	Trace(RightSocket, RightDistance);
}

void UCFeetComponent::Trace(FName InSoket, float& OutDistance)
{
	OutDistance = 0.f;

	FVector SocketLocation = OwnerCharacter->GetMesh()->GetSocketLocation(InSoket);
	FVector Start(SocketLocation.X, SocketLocation.Y, OwnerCharacter->GetActorLocation().Z);

	float CapsuleHalfHeight = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	float TraceZ =  Start.Z - CapsuleHalfHeight - Additional;

	FVector End(Start.X, Start.Y, TraceZ);

	TArray<AActor*> Ignores;
	Ignores.Add(OwnerCharacter);

	FHitResult Hit;
	UKismetSystemLibrary::LineTraceSingle
	(
		GetWorld(),
		Start,
		End,
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility),
		true,
		Ignores,
		DrawDebugType,
		Hit,
		true
	);

	CheckFalse(Hit.IsValidBlockingHit());

	float DigLength = (Hit.ImpactPoint - Hit.TraceEnd).Size();
	OutDistance = FootHeight + DigLength - Additional;
	//Todo. 
}

