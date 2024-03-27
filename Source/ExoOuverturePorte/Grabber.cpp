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

	ActorHasTag = false;
	TagToSearch = "Chaise";
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

		if (CurrentValueGrab <= 0) {
			PhysicsHandleComponent->ReleaseComponent();
		}
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
			TimerFreeze = 5.f;
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
		InputComponent->BindAction("RotateObjectRoll", IE_Pressed, this, &UGrabber::RotateObjectRollPressed);
		InputComponent->BindAction("RotateObjectYaw", IE_Pressed, this, &UGrabber::RotateObjectYawPressed);
		InputComponent->BindAction("InverseRotation", IE_Pressed, this, &UGrabber::ReverseRotationPressed);
		InputComponent->BindAction("Throw", IE_Pressed, this, &UGrabber::Throw);
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

FRotator UGrabber::GetPlayersWorldRotation() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint
	(
		PlayerViewPointLocation,
		PlayerViewPointRotation
	);
	return PlayerViewPointRotation;
}



float UGrabber::GetValueUI()
{
	return CurrentValueGrab / MaxValueGrab;
}

bool UGrabber::GetObjectQuestGrab()
{
	return ActorHasTag;
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab"));

	 HitResult = GetFirstPhysicsBodyInReach();

	 ComponentToGrab = HitResult.GetComponent();

	 ActorHit = HitResult.GetActor();

	if (ActorHit && CurrentValueGrab > 0) {
		PhysicsHandleComponent->GrabComponentAtLocation(
			ComponentToGrab,
			NAME_None,
			GetPlayersReach()
		);

		/*PhysicsHandleComponent->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			NAME_None,
			GetPlayersReach(),
			ActorHit->GetActorRotation()
		);*/
		ComponentToGrab->SetSimulatePhysics(true);
		isFreezed = false;
		IsGrabbed = true;

		if (ActorHit->ActorHasTag(TagToSearch)) {
			ActorHasTag = true;
		}
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Release"));
	PhysicsHandleComponent->ReleaseComponent();
	IsGrabbed = false;

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

void UGrabber::RotateObjectRollPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("E Pressed"));

	if (IsGrabbed) {
		/*FRotator ActorRotation = ActorHit->GetActorRotation();
		FRotator NewActorRotation(0.f, 45.f, 0.f);
		ActorRotation += NewActorRotation;*/
		ComponentToGrab->SetSimulatePhysics(false);
		FRotator NewRotation = ActorHit->GetActorRotation().Add(0.f, 45.f, 0.f);
		ActorHit->SetActorRotation(NewRotation);
		ComponentToGrab->SetSimulatePhysics(true);
	}
}

void UGrabber::RotateObjectYawPressed()
{
	UE_LOG(LogTemp, Warning, TEXT("R Pressed"));
	if (IsGrabbed) {
		ComponentToGrab->SetSimulatePhysics(false);
		FRotator NewRotation = ActorHit->GetActorRotation().Add(0.f, 0.f, 45.f);
		ActorHit->SetActorRotation(NewRotation);
		ComponentToGrab->SetSimulatePhysics(true);
	}
}

void UGrabber::ReverseRotationPressed()
{
	IsLSPressed = true;
}

void UGrabber::Throw()
{
	UE_LOG(LogTemp, Warning, TEXT("Left Mouse Pressed"));
	if (IsGrabbed) {
		PhysicsHandleComponent->ReleaseComponent();

		APlayerCameraManager* PlayerCamera = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
		FRotator CameraRot = PlayerCamera->GetCameraRotation();
		FVector CameraForward = CameraRot.Vector();
		/// (Using player's forward vector to determine direction of throw).
		ComponentToGrab->AddImpulse(
			CameraForward * 1000,
			NAME_None, /// No bone names for specific  objects.
			true /// Makes sure that mass is immaterial to the force.
		);
	}
}



