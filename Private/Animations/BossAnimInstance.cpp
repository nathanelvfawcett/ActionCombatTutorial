// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/BossAnimInstance.h"


void UBossAnimInstance::UpdateSpeed() {
	APawn* pawnRef{ TryGetPawnOwner() };
	if (!IsValid(pawnRef)) {
		return;
	}

	FVector Velcocity{ pawnRef->GetVelocity() };
	CurrentSpeed = static_cast<float>(Velcocity.Length());

}