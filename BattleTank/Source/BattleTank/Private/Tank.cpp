// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankMovementComponent.h"

// Sets default values
ATank::ATank()
{
	
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
}

float ATank::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) 
{	
	int32 DamagePoints = FPlatformMath::RoundToInt(DamageAmount);
	int32 DamageToApply = FMath::Clamp(DamagePoints, 0, CurrentHealth);

	CurrentHealth -= DamageToApply;
	if (CurrentHealth <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tank DIED"));
	}

	return DamageToApply;
}