// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "Components/AudioComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLASESEM2_3_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();
	

private:
	float InitialYaw;
	float CurrentYaw;

	bool IsDoorOpen = false;
	
	UPROPERTY(EditAnywhere)
	float MassToOpenDoors = 50.f;
	
	UPROPERTY(EditAnywhere)
	float OpenAngle = 90.f;
	float DoorLastOpened = 0.f;

	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = .5f;

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = .5f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 2.f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* Pressure;

	UPROPERTY(EditAnywhere)
	AActor* ActorOpen;

	UPROPERTY(EditAnywhere)
	UAudioComponent* AudioComponent = nullptr;

	void InitialValues();
	void SetActor();
	float TotalMassOfActors() const;

	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	void SetAudioComponent();
	void PlaySound();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
