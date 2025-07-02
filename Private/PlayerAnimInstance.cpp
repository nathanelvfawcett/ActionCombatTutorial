// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"

void UPlayerAnimInstance::UpdateSpeed() {
	APawn* pawnRef{ TryGetPawnOwner() };
	if (!IsValid(pawnRef)) {
		return;
	}

	FVector Velcocity{ pawnRef->GetVelocity() };
	currentSpeed = static_cast<float>(Velcocity.Length());

}

void UPlayerAnimInstance::HandleUpdatedTarget(AActor* NewTargetActorRef)
{
	bIsInCombat = IsValid(NewTargetActorRef);
}

void UPlayerAnimInstance::UpdateDirection()
{
	APawn* PawnRef{ TryGetPawnOwner() };
	if (!IsValid(PawnRef)) { return; }
	if(!bIsInCombat) { return; }
	currentDirection = CalculateDirection(
		PawnRef->GetVelocity(),
		PawnRef->GetActorRotation()
	);
}
