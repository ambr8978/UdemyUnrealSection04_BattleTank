// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

class AProjectile;

UENUM()
enum class EFiringState : uint8
{
	AIMING,
	LOCKED,
	RELOADING
};

class UTankBarrel;
class UTankTurret;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTankAimingComponent();

	void SetBarrelReference(UTankBarrel* BarrelToSet);
	void SetTurretReference(UTankTurret* TurretToSet);

	void Fire();
	void AimAt(FVector HitLocation);
	void MoveBarrelTowards(FVector AimDirection);
protected:
	UPROPERTY(EditDefaultsOnly, Category = Firing)
		float LaunchSpeed = 4000; //1000 m/s

	UPROPERTY(EditDefaultsOnly, Category = Firing)
		float ReloadTimeInSeconds = 3.0f;

	UPROPERTY(BlueprintReadOnly, Category = State)
		EFiringState FiringState = EFiringState::RELOADING;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
		TSubclassOf<AProjectile> ProjectileBP = nullptr;

	double LastFireTime = 0;
	UTankTurret* Turret = nullptr;
	UTankBarrel* Barrel = nullptr;

	// Called when the game starts
	virtual void BeginPlay() override;
};
