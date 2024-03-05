// Fill out your copyright notice in the Description page of Project Settings.


#include "PositionReport.h"

// Sets default values for this component's properties
UPositionReport::UPositionReport()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPositionReport::BeginPlay()
{
	Super::BeginPlay();

	//Get Owner of this scritps
	GetOwner();
	//Get the name of the owner in the scene
	FString OwnName = GetOwner()->GetActorLabel();
	//Get position of the owner
	FString OwnLocation = GetOwner()->GetActorLocation().ToString();
	FString OwnRotation = GetOwner()->GetActorRotation().ToString();

	UE_LOG(LogTemp, Warning, TEXT("%s is at %s and rotation %s"), *OwnName, *OwnLocation, *OwnRotation);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White , FString::Printf(TEXT("%s is at %s and rotation %s"), *OwnName, *OwnLocation, *OwnRotation));
	// ...
	
}


// Called every frame
void UPositionReport::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

