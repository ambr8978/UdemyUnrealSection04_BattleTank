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

	UFUNCTION(BlueprintCallable, Category = Setup)
	void Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);

	UFUNCTION(BlueprintCallable, Category = Firing)
	void Fire();
	void AimAt(FVector HitLocation);

protected:
	UPROPERTY(BlueprintReadOnly, Category = State)
		EFiringState FiringState = EFiringState::RELOADING;

private:
	UPROPERTY(EditDefaultsOnly, Category = Firing)
		float LaunchSpeed = 4000; //1000 m/s

	UPROPERTY(EditDefaultsOnly, Category = Firing)
		float ReloadTimeInSeconds = 3.0f;


	UPROPERTY(EditDefaultsOnly, Category = Setup)
		TSubclassOf<AProjectile> ProjectileBP = nullptr;

	double LastFireTime = 0;
	UTankTurret* Turret = nullptr;
	UTankBarrel* Barrel = nullptr;

	virtual void BeginPlay() override;
	void MoveBarrelTowards(FVector AimDirection);
};
