// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Characters/EStat.h"
#include "StatsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnHealthUpdateSignature,
	UStatsComponent, OnHealthPercentageUpdateDelegate,
	float, Percentage
);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnStaminaUpdateSignature,
	UStatsComponent, OnStaminaPercentageUpdateDelegate,
	float, Percentage
);

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(
	FOnZeroHealthSignature,
	UStatsComponent, OnZeroHealthDelegate
);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONCOMBATTUTORIAL_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	double StaminaRegenRate{10.0};

	UPROPERTY(VisibleAnywhere)
	bool bCanRegenStamina{ true };
	UPROPERTY(EditAnywhere)
	float StaminaDelayDuration{ 2.f };

public:	
	// Sets default values for this component's properties
	UStatsComponent();
	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<EStat>, float> Stats;

	UPROPERTY(BlueprintAssignable)
	FOnHealthUpdateSignature OnHealthPercentageUpdateDelegate;
	UPROPERTY(BlueprintAssignable)
	FOnStaminaUpdateSignature OnStaminaPercentageUpdateDelegate;
	
	UPROPERTY(BlueprintAssignable)
	FOnZeroHealthSignature OnZeroHealthDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void ReduceHealth(float Amount, AActor* Opponent);

	UFUNCTION(BlueprintCallable)
	void ReduceStamina(float Amount);

	UFUNCTION(BlueprintCallable)
	void RegenStamina();

	UFUNCTION()
	void EnableStaminaRegen();

	UFUNCTION(BlueprintPure)
	float GetStatPercentage(EStat Current, EStat Max);
		
};
