// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"

UTankTrack::UTankTrack() 
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankTrack::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	auto RightVector = GetRightVector();

	auto SlippageSpeed = FVector::DotProduct(RightVector, GetComponentVelocity());
	auto CorrectionAcceleration = -(SlippageSpeed / DeltaTime*RightVector);

	auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
	//Divide by two since  we have two tracks
	auto CorrectionForce = (TankRoot->GetMass() * CorrectionAcceleration) / 2; 
	TankRoot->AddForce(CorrectionForce);
}

void UTankTrack::SetThrottle(float Throttle)
{
	FVector ForceToApply = GetForwardVector() * Throttle * TrackMaxDrivingForce;
	FVector ForceLocation = GetComponentLocation();
	UPrimitiveComponent* TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceToApply, ForceLocation);
}

