// Fill out your copyright notice in the Description page of Project Settings.


#include "Compass.h"
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UCompass::UCompass()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCompass::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//Récuperer l'objectif
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("objectif"), OutActors);
	int num = OutActors.Num();
	if (num > 0) {
		goalActor = OutActors[0];
	}
}


// Called every frame
void UCompass::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	// Récuperer la position du player
	FVector posPlayer = GetOwner()->GetActorLocation();

	if (goalActor == nullptr) return;
	// Récupérer position objectif
	FVector posGoal = goalActor->GetActorLocation();

	// Trouver la direction depuis le player vers l'objectif
	FVector directionGoal = posGoal - posPlayer;

	// Normalizer la direction
	directionGoal.Normalize();

	// Modifier les 2 typage de direction en rotation
	FRotator anglePlayer;
	FVector tkt;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT tkt, OUT anglePlayer);
	FRotator angleGoal = directionGoal.Rotation();

	//Calculer le Delta entre les rotations
	FRotator deltaAngle = anglePlayer - angleGoal;

	angleUIdegree = deltaAngle.Yaw;

}

float UCompass::GetAngleUI()
{
	return angleUIdegree;
}



