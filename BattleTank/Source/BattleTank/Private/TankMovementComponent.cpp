// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"

void UTankMovementComponent::Initialize(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet)
{
	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;
}

void UTankMovementComponent::IntentMoveForward(float Throw)
{
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);

	//TODO prevent double input from stick and throttles
}

void UTankMovementComponent::IntentTurnRight(float Throw)
{
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(-Throw);
}

void UTankMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed)
{
	// Explicitly overriding UNavComponent's implementation, so no Super call
	auto TankDirectionNormal = GetOwner()->GetActorForwardVector().GetSafeNormal();
	auto AiMoveIntentionNormal = MoveVelocity.GetSafeNormal();

	auto ForwardThrowNormal = FVector::DotProduct(TankDirectionNormal, AiMoveIntentionNormal);
	IntentMoveForward(ForwardThrowNormal);

	auto RightTurnNormal = FVector::CrossProduct(TankDirectionNormal, AiMoveIntentionNormal).Z;
	IntentTurnRight(RightTurnNormal);
}