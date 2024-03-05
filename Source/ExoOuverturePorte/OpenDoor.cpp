// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (PressurePlate) {
		PressurePlate->GetOverlappingActors(OUT CollectedActors);

		for (const auto& Actor : CollectedActors) {
			if (GetMassInPressurePlate() >= LimitMassBeforeOpen) {
				OpenDoor();
				count = 0;
			}

			count += DeltaTime;
			if (count >= DoorCloseDelay) {
				CloseDoor();
			}
		}
	}
}

void UOpenDoor::OpenDoor()
{
	OnOpenResquest.Broadcast();
}

void UOpenDoor::CloseDoor()
{
	OnCloseResquest.Broadcast();
}

float UOpenDoor::GetMassInPressurePlate()
{
	if (!PressurePlate) return 0.0f;
	float MassTotal = 0.0f;
	for (const auto& Actor : CollectedActors) {
		float MassOfTheCurrentActor = Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		MassTotal += MassOfTheCurrentActor;
	}
	return MassTotal;
}


