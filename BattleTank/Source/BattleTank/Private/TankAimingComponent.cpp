// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h "

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{	
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankAimingComponent::BeginPlay()
{
	LastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet) {
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	if (RoundsLeft <= 0)
	{
		FiringState = EFiringState::OUT_OF_AMMO;
	}
	else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds)
	{
		FiringState = EFiringState::RELOADING;
	}
	else if (IsBarrelMoving())
	{
		FiringState = EFiringState::AIMING;
	}
	else
	{
		FiringState = EFiringState::LOCKED;
	}
}

EFiringState UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) {return true;}
	auto BarrelForward = Barrel->GetForwardVector();
	return !(BarrelForward.Equals(AimDirection, 0.01));
}

void UTankAimingComponent::SetBarrelReference(UTankBarrel* BarrelToSet)
{
	Barrel = BarrelToSet;
}

void UTankAimingComponent::SetTurretReference(UTankTurret* TurretToSet)
{
	Turret = TurretToSet;
}

int32 UTankAimingComponent::GetRoundsLeft() const
{
	return RoundsLeft;
}

void UTankAimingComponent::Fire()
{
	if (FiringState == EFiringState::LOCKED || FiringState == EFiringState::AIMING)
	{
		if (!ensure(Barrel)) { return; }
		if (!ensure(ProjectileBP)) { return; }

		// Spawn a projectile at the socket location on the barrel
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBP,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
			);

		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
		--RoundsLeft;
	}
}

/*
  We are getting the aiming direction by using the ray trace HitLocation (passed in as a parameter)
  along with our starting location (Barrel socket location) and projectile Launch speed (UPROPERTY set by designers) 
  and plugging it into UGameplayStatics.SuggestProjectileVelocity to get the normal of the suggested velocity.

  This normal is the Aiming direction.
*/
void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!Barrel)
	{
		UE_LOG(LogTemp, Error, TEXT("TankAimingComponenet.AimAt - Barrel is null"));
	}

	FVector OutLaunchVelocity;
	/*
	TODO
	Looks like we could potentially have an issue if the Barrel had multiple sockets with the same name
	I'd prefer making the Projectile socket name a const std::string
	*/
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

	bool bAimDirectionEstablished = UGameplayStatics::SuggestProjectileVelocity(
			this,
			OutLaunchVelocity,
			StartLocation,
			HitLocation,
			LaunchSpeed,
			false,
			0,
			0,
			ESuggestProjVelocityTraceOption::DoNotTrace
		);

	//Calculate the OutLaunchVelocity
	if (bAimDirectionEstablished)
	{
		AimDirection = OutLaunchVelocity.GetSafeNormal();
		auto TankName = GetOwner()->GetName();
		MoveBarrelTowards(AimDirection);
	}
 }

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	//Work-out the difference between current barrel rotation and AimDirection
	//This tells us the roll, pitch and yaw of the Barrel.
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;
	
	Barrel->Elevate(DeltaRotator.Pitch);
	if (FMath::Abs(DeltaRotator.Yaw) < 180)
	{
		Turret->Rotate(DeltaRotator.Yaw);
	}
	else
	{
		Turret->Rotate(-DeltaRotator.Yaw);
	}
}

