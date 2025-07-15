// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONCOMBATTUTORIAL_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float currentSpeed{ 0.f };

	UFUNCTION(BlueprintCallable)
	void UpdateSpeed();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsInCombat{ false };
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float currentDirection{ 0.f };

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsBlocking{ false };

	UFUNCTION(BlueprintCallable)
	void HandleUpdatedTarget(AActor* NewTargetActorRef);

	UFUNCTION(BlueprintCallable)
	void UpdateDirection();
	
};
