// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tank.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

#define CROSSHAIR_DEFAULT_X .5
#define CROSSHAIR_DEFAULT_Y .33333

UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(EditAnywhere)
		float LineTraceRange = 1000000.0f;

	UPROPERTY(EditAnywhere)
		float CrosshairXLocation = CROSSHAIR_DEFAULT_X;

	UPROPERTY(EditAnywhere)
		float CrosshairYLocation = CROSSHAIR_DEFAULT_Y;

	ATank* GetControlledTank() const;
	void AimTowardsCrosshair();
	/*
	Return true if ray hit is hitting landscape.
	Ray trace.
	*/
	bool GetLookVectorHitLocation(FVector& LookDirection, FVector& HitLocation) const;
	bool GetSightRayHitLocation(FVector& HitLocation) const;
	bool GetLookDirection(FVector2D CrosshairScreenLocation, FVector& LookDirection) const;
};
