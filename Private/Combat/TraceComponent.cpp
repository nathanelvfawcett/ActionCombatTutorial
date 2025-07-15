// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/TraceComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
UTraceComponent::UTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	SkeletalComp = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();

	// ...
	
}


// Called every frame
void UTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsAttacking) { return; }

	TArray<FHitResult> AllResults;
	for (const FTraceSockets Socket: Sockets) {
		FVector StartSocketLocation{ SkeletalComp->GetSocketLocation(Socket.Start) };
		FVector EndSocketLocation{ SkeletalComp->GetSocketLocation(Socket.End) };
		FQuat ShapeRotation{ SkeletalComp->GetSocketQuaternion(Socket.Rotation) };

		TArray<FHitResult> OutResults;
		double WeaponDistance{
			FVector::Distance(StartSocketLocation,EndSocketLocation)
		};
		FVector BoxHalfExtent{
			BoxSollisionLength, BoxSollisionLength, WeaponDistance
		};
		BoxHalfExtent /= 2;
		FCollisionShape Box{
			FCollisionShape::MakeBox(BoxHalfExtent)
		};

		FCollisionQueryParams IgnoreParams{
			FName {TEXT("Ignore Params")},
			false,
			GetOwner()
		};

		bool bHasFoundTargets{ GetWorld()->SweepMultiByChannel(
			OutResults,
			StartSocketLocation,
			EndSocketLocation,
			ShapeRotation,
			ECollisionChannel::ECC_GameTraceChannel1,
			Box,
			IgnoreParams
		) };

		for (FHitResult Hit : OutResults) {
			AllResults.Add(Hit);
		}

		if (bDebugMode) {
			FVector CenterPoint{
				UKismetMathLibrary::VLerp(
					StartSocketLocation, EndSocketLocation, .5f
				)
			};

			UKismetSystemLibrary::DrawDebugBox(
				GetWorld(),
				CenterPoint,
				Box.GetExtent(),
				bHasFoundTargets ? FLinearColor::Green : FLinearColor::Red,
				ShapeRotation.Rotator(),
				1.f,
				2.f
			);

		}
	}

	if (AllResults.Num()==0) { return; }
	float CharacterDamage{ 0.f };
	IFighter* FighterRef{ Cast<IFighter>(GetOwner()) };
	if (FighterRef) {
		CharacterDamage = FighterRef->GetDamage();
	}
	//UE_LOG(LogTemp, Warning, TEXT("Damage: %f"), CharacterDamage);
	FDamageEvent TargetAttackEvent;
	for (const FHitResult& Hit : AllResults) {
		AActor* TargetActor{Hit.GetActor()};
		if (TargetsToIgnore.Contains(TargetActor)) { continue; }
		TargetActor->TakeDamage(CharacterDamage, TargetAttackEvent,
			GetOwner()->GetInstigatorController(),
			GetOwner());
		TargetsToIgnore.AddUnique(TargetActor);
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticleTemplate, Hit.ImpactPoint);
	}

}

void UTraceComponent::HandleResetAttack()
{
	TargetsToIgnore.Empty();
}


