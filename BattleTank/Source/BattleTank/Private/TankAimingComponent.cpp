// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h "

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTankAimingComponent::SetBarrelReference(UTankBarrel* BarrelToSet)
{
	Barrel = BarrelToSet;
}

/*
  We are getting the aiming direction by using the ray trace HitLocation (passed in as a parameter)
  along with our starting location (Barrel socket location) and projectile Launch speed (UPROPERTY set by designers) 
  and plugging it into UGameplayStatics.SuggestProjectileVelocity to get the normal of the suggested velocity.

  This normal is the Aiming direction.
*/
void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed)
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
		auto AimDirection = OutLaunchVelocity.GetSafeNormal();
		auto TankName = GetOwner()->GetName();
		UE_LOG(LogTemp, Warning, TEXT("%s aiming at %s"), *TankName, *(AimDirection.ToString()))
		MoveBarrelTowards(AimDirection);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No aim direction established"))
	}
 }

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	//Work-out the difference between current barrel rotation and AimDirection
	//This tells us the roll, pitch and yaw of the Barrel.
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;
	
	Barrel->Elevate(5);
}

