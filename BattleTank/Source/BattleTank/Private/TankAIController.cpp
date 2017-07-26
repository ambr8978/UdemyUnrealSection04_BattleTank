// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
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
	if (PlayerTank)
	{
		//Move towards the player

		//Aim towards the player
		auto AiControlledTank = Cast<ATank>(GetPawn());
		AiControlledTank->AimAt(PlayerTank->GetActorLocation());

		//Fire if ready
		AiControlledTank->Fire();
	}
}
