// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AI/BTT_RangeAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "Characters/EEnemyState.h"

EBTNodeResult::Type UBTT_RangeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* CharacterRef{ OwnerComp.GetAIOwner()->GetPawn<ACharacter>()};

	if (!IsValid(CharacterRef)) { return EBTNodeResult::Failed; }
	float Distance{ OwnerComp.GetBlackboardComponent()->GetValueAsFloat(TEXT("Distance"))};
	if (Distance < MeleeRange) {
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::Melee);
		AbortTask(OwnerComp, NodeMemory);
		return EBTNodeResult::Aborted;
	}
	CharacterRef->PlayAnimMontage(AnimMontage);
	double RandomValue{UKismetMathLibrary::RandomFloat()};
	if (RandomValue > Threshold) {
		Threshold = 0.9f;
		OwnerComp.GetBlackboardComponent()->SetValueAsEnum(TEXT("CurrentState"), EEnemyState::Charge);
	}
	else {
		Threshold -= .1;
	}
	return EBTNodeResult::Succeeded;
}
