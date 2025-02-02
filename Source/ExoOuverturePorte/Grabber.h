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
	UFUNCTION(BlueprintCallable)
	bool GetObjectQuestGrab();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SetUpInputC();
	void FindAttachPhysicsHandle();
	FHitResult GetFirstPhysicsBodyInReach()const;
	FVector GetPlayersReach() const;
	FVector GetPlayersWorldPosition() const;
	FRotator GetPlayersWorldRotation() const;

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
	bool IsGrabbed = false;

	FHitResult HitResult;
	UPrimitiveComponent* ComponentToGrab = nullptr;
	AActor* ActorHit = nullptr;

	FRotator ActorRotation;

	bool isFreezed = false;
	float TimerFreeze = 5.0f;

	bool CanFreeze = false;
	float MaxCooldownFreezeValue = 3.0f;
	float CurrentCooldownFreezeValue;

	bool IsEPressed = false;
	bool IsRPressed = false;
	bool IsLSPressed = false;

	bool ActorHasTag = false;
	FName TagToSearch;

	UFUNCTION(BlueprintCallable)
	void Grab();
	void Release();
	void Freeze();
	void RotateObjectRollPressed();
	void RotateObjectYawPressed();
	void ReverseRotationPressed();
	void Throw();
};
