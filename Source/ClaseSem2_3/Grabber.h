// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLASESEM2_3_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float Reach = 100.f;
	UPhysicsHandleComponent* PhysicsHandler = nullptr;
	UInputComponent* InputComponent = nullptr;
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	void SetInputComponent();
	void Grab();
	void Release();
	void CheckCollision();
	void FindPhysicsHandle();
	FHitResult GetFirstPhysicsBodyInReach();
	FVector GetLineTraceEnd();
	FVector GetPlayersReach() const;
	FVector GetPlayersWorldPos() const;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
