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

	InterpSpeed = 10.f;

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
	FRotator LeftRotation;
	Trace(LeftSocket, LeftDistance, LeftRotation);

	float RightDistance;
	FRotator RightRotation;
	Trace(RightSocket, RightDistance, RightRotation);

	float Floating = FMath::Min(LeftDistance, RightDistance);

	//World Space
	Data.PelvisDistance.Z =  UKismetMathLibrary::FInterpTo(Data.PelvisDistance.Z, Floating, DeltaTime, InterpSpeed);

	//Bone Space
	Data.LeftDistance.X = UKismetMathLibrary::FInterpTo(Data.LeftDistance.X, LeftDistance - Floating, DeltaTime, InterpSpeed); ;
	Data.RightDistance.X = UKismetMathLibrary::FInterpTo(Data.RightDistance.X, -(RightDistance - Floating), DeltaTime, InterpSpeed);

	//World Space
	Data.LeftRotation = UKismetMathLibrary::RInterpTo(Data.LeftRotation, LeftRotation, DeltaTime, InterpSpeed);
	Data.RightRotation = UKismetMathLibrary::RInterpTo(Data.RightRotation, RightRotation, DeltaTime, InterpSpeed);
}

void UCFeetComponent::Trace(FName InSoket, float& OutDistance, FRotator& OutRotation)
{
	OutDistance = 0.f;
	OutRotation = FRotator::ZeroRotator;

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

	FVector ImpactNormal = Hit.ImpactNormal;
	float Pitch = -UKismetMathLibrary::DegAtan2(ImpactNormal.X, ImpactNormal.Z);
	float Roll = UKismetMathLibrary::DegAtan2(ImpactNormal.Y, ImpactNormal.Z);

	Pitch = FMath::Clamp(Pitch, -30.f, 30.f);
	Roll = FMath::Clamp(Roll, -15.f, 15.f);

	 FRotator ImpactRotation(Pitch, 0.f, Roll);

	OutRotation = ImpactRotation;
}

