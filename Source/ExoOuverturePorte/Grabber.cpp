// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	SetUpInputC();
	FindAttachPhysicsHandle();
	
	CurrentValueGrab = MaxValueGrab;

	CurrentCooldownFreezeValue = MaxCooldownFreezeValue;
	CanFreeze = true;
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	

	if (PhysicsHandleComponent->GrabbedComponent && CurrentValueGrab > 0)
	{
		CurrentValueGrab -= DeltaTime * SpeedGrabUse;
		// Move the object that we are currently holding
		PhysicsHandleComponent->SetTargetLocation(GetPlayersReach());
	}
	else 
	{
		CurrentValueGrab += DeltaTime * SpeedGrabUse;
	}

	if (CanFreeze == false) {
		CurrentCooldownFreezeValue -= DeltaTime;
		if (CurrentCooldownFreezeValue <= 0) {
			CurrentCooldownFreezeValue = MaxCooldownFreezeValue;
			CanFreeze = true;
		}
	}

	if (isFreezed == true) {
		TimerFreeze -= DeltaTime;
		if (TimerFreeze <= 0) {
			ComponentToGrab->SetSimulatePhysics(true);
			isFreezed = false;
		}
	}

	//FMath::Clamp(CurrentValueGrab, 0.f, MaxValueGrab);
}

void UGrabber::SetUpInputC()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("InputComponent not initialized"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("InputComponent initialized"));
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
		InputComponent->BindAction("Freeze", IE_Pressed, this, &UGrabber::Freeze);
	}
}

void UGrabber::FindAttachPhysicsHandle()
{
	PhysicsHandleComponent = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandleComponent == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("PhysicsComponent not initialized"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("PhysicsComponent initialized"));
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FHitResult Hit;
	// Ray Cast out to a certain distance (Reach)
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType
	(
		Hit,
		GetPlayersWorldPosition(),
		GetPlayersReach(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);
	return Hit;
}

FVector UGrabber::GetPlayersReach() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
		PlayerViewPointLocation,
		PlayerViewPointRotation
	);
	return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * ReachDistanceLine;
}

FVector UGrabber::GetPlayersWorldPosition() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
		PlayerViewPointLocation,
		PlayerViewPointRotation
	);
	return PlayerViewPointLocation;
}



float UGrabber::GetValueUI()
{
	return CurrentValueGrab / MaxValueGrab;
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab"));

	 HitResult = GetFirstPhysicsBodyInReach();

	 ComponentToGrab = HitResult.GetComponent();

	 ActorHit = HitResult.GetActor();

	if (ActorHit) {
		PhysicsHandleComponent->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			GetPlayersReach()
		);
		ComponentToGrab->SetSimulatePhysics(true);
		isFreezed = false;
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Release"));
	PhysicsHandleComponent->ReleaseComponent();

}

void UGrabber::Freeze()
{
	UE_LOG(LogTemp, Warning, TEXT("Freeze"));
	if (isFreezed == false && CanFreeze == true) {
		ComponentToGrab->SetSimulatePhysics(false);
		isFreezed = true;
		CanFreeze = false;
	}
}

