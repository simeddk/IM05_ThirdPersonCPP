#include "CBTTaskNode_Speed.h"
#include "Global.h"
#include "AIController.h"

UCBTTaskNode_Speed::UCBTTaskNode_Speed()
{
    NodeName = "Speed";
}

EBTNodeResult::Type UCBTTaskNode_Speed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AAIController* AIC = OwnerComp.GetAIOwner();
    CheckNullResult(AIC, EBTNodeResult::Failed);

    //Todo.
    /*UCAttributeComponent*  CHelpers::GetComponent<UCAttributeComponent>(AIC->GetPawn());


    AttributeComp

    return EBTNodeResult::Type();*/
}
