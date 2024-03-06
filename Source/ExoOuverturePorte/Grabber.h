// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "CoreMinimal.h"
#include "Math/UnrealMathUtility.h"
#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EXOOUVERTUREPORTE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	UFUNCTION(BlueprintCallable)
	float GetValueUI();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SetUpInputC();
	void FindAttachPhysicsHandle();
	FHitResult GetFirstPhysicsBodyInReach()const;
	FVector GetPlayersReach() const;
	FVector GetPlayersWorldPosition() const;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere)
	UInputComponent* InputComponent = nullptr;
	UPhysicsHandleComponent* PhysicsHandleComponent = nullptr;
	UPROPERTY(EditAnywhere)
	float ReachDistanceLine = NULL;
	UPROPERTY(EditAnywhere)
	float MaxValueGrab = 10.0f;
	float CurrentValueGrab = 0.0f;

	float SpeedGrabUse = 1.0f;

	FHitResult HitResult;
	UPrimitiveComponent* ComponentToGrab = nullptr;
	AActor* ActorHit = nullptr;

	bool isFreezed = false;
	float TimerFreeze = 5.0f;

	UFUNCTION(BlueprintCallable)
	void Grab();
	void Release();
	void Freeze();
};
