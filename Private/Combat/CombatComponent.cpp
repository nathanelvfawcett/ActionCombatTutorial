// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/CombatComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interfaces/MainPlayer.h"


// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	CharacterRef = GetOwner<ACharacter>();
	
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCombatComponent::ComboAttack()
{
	if (CharacterRef->Implements<UMainPlayer>()) {
		IMainPlayer* IPlayerRef{ Cast<IMainPlayer>(CharacterRef) };

		if (IPlayerRef && !IPlayerRef->HasEnoughStamina(StaminaCost)) {
			return;
		}
	}
	if (!bCanAttack) { return; }
	bCanAttack = false;
	CharacterRef->PlayAnimMontage(AttackAnimations[comboCount]);
	comboCount++;
	int MaxCombo{ AttackAnimations.Num() };
	comboCount = UKismetMathLibrary::Wrap(
		comboCount,
		-1,
		(MaxCombo-1)
	);

	OnAttackPerformedDelegate.Broadcast(StaminaCost);
}

void UCombatComponent::HandleResetAttack()
{
	bCanAttack = true;
}

void UCombatComponent::RandomAttack()
{
	int RandomIndex{ FMath::RandRange(0, AttackAnimations.Num()-1) };
	AnimDuration = CharacterRef->PlayAnimMontage(AttackAnimations[RandomIndex]);
}

