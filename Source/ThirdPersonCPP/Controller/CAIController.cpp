#include "CAIController.h"
#include "Global.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Characters/CEnemy_AI.h"

ACAIController::ACAIController()
{
	CHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "Blackboard");
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	PossessedEnemy = Cast<ACEnemy_AI>(InPawn);
	UseBlackboard(PossessedEnemy->GetBehaviorTree()->GetBlackboardAsset(), Blackboard);

	RunBehaviorTree(PossessedEnemy->GetBehaviorTree());
	//Todo. Build -> Play -> BT 실행이 되어야 함
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ACAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
