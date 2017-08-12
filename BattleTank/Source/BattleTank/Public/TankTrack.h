// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrack.generated.h"

UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = Input)
		void SetThrottle(float Throttle);

private:
	//Max force per track in Newtons
	//Assume 1g acceleration and 40 tonne tank
	UPROPERTY(EditDefaultsOnly)
		float TrackMaxDrivingForce = 400000;	

	/*
	This function will get called every frame that the TankTrack is hit 
	(which is likely every frame since the Tank should usually be on the ground)
	*/
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	float CurrentThrottle = 0.0f;

	UTankTrack();
	void BeginPlay() override;
	void ApplySidewaysForce();
	void DriveTrack();
};
