// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
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
	InitialValues();
}

void UOpenDoor::InitialValues(){
	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;
	if(!Pressure){
		UE_LOG(LogTemp,Error,TEXT("%s no plate"), *GetOwner()->GetName());
	}
	SetActor();
	SetAudioComponent();
}

void UOpenDoor::SetActor(){
	ActorOpen = GetWorld()->GetFirstPlayerController()->GetPawn();
	if(!ActorOpen){
		UE_LOG(LogTemp,Error,TEXT("No hay player"));
	}
}

void UOpenDoor::OpenDoor(float DeltaTime){
    CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngle, DeltaTime*DoorOpenSpeed);
    FRotator DoorRotation = GetOwner()->GetActorRotation();
    DoorRotation.Yaw = CurrentYaw;
    GetOwner()->SetActorRotation(DoorRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime){
    CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime*DoorCloseSpeed);
    FRotator DoorRotation = GetOwner()->GetActorRotation();
    DoorRotation.Yaw = CurrentYaw;
    GetOwner()->SetActorRotation(DoorRotation);
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if(TotalMassOfActors() > MassToOpenDoors){
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
		if(!IsDoorOpen){
			IsDoorOpen = true;
			PlaySound();
		}
	} else{
		if(GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay){
			CloseDoor(DeltaTime);
			if(IsDoorOpen){
				IsDoorOpen = false;
				PlaySound();
			}
		}
	}
}

void UOpenDoor::PlaySound(){
	if(AudioComponent)
		AudioComponent->Play();
}

void UOpenDoor::SetAudioComponent(){
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if(!AudioComponent){
		UE_LOG(LogTemp,Error,TEXT("%s no audiocomponent"),*GetOwner()->GetName());
	}
}

float UOpenDoor::TotalMassOfActors() const{
	float TotalMass = 0.f;
	TArray<AActor*> OverlappingActor;
	if(Pressure){
		Pressure->GetOverlappingActors(OUT OverlappingActor);
		for(AActor* Actor : OverlappingActor){
			TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
			UE_LOG(LogTemp,Error,TEXT("%s is on pressure"), *Actor->GetName());
		}
	}
	return TotalMass;
}