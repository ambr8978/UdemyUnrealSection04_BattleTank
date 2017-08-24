// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "Tank.h"
#include "Engine.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	auto ControlledTank = GetPawn();
	if (!ensure(PlayerTank)) { return; }
	if (!ensure(ControlledTank)) { return; }

	//Move towards the player
	MoveToActor(PlayerTank, AcceptanceRadius, true, true, false);

	auto AimingComponent = ControlledTank->FindComponentByClass<UTankAimingComponent>();
	AimingComponent->AimAt(PlayerTank->GetActorLocation());

	if (AimingComponent->GetFiringState() == EFiringState::LOCKED)
	{
		AimingComponent->Fire();
	}
}

void ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	ATank* PossessedTank = Cast<ATank>(InPawn);

	if (!ensure(PossessedTank))
	{
		return;
	}

	PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossessedTankDeath);
}

void ATankAIController::OnPossessedTankDeath()
{
	if (!GetPawn())
	{
		return;
	}

	GetPawn()->DetachFromControllerPendingDestroy();
}