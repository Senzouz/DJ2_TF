// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"

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

	SetInputComponent();
	FindPhysicsHandle();
}

void UGrabber::SetInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Error, TEXT("Grab called"));
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	if (HitResult.GetActor())
	{
		FVector LineTraceEnd = GetLineTraceEnd();
		UPrimitiveComponent *ComponentToGrab = HitResult.GetComponent();
		PhysicsHandler->GrabComponentAtLocation(ComponentToGrab, NAME_None, LineTraceEnd);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No hit"));
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Error, TEXT("Release called"));
	PhysicsHandler->ReleaseComponent();
}

void UGrabber::CheckCollision()
{
	//FVector LineTraceEnd = GetLineTraceEnd();

	/*DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(0,255,0),
                  false, 0.f, 0.f, 5.0f);*/
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandler = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandler)
	{
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No physics handler found on %s"), *GetOwner()->GetName());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector LineTraceEnd = GetLineTraceEnd();

	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, PlayerViewPointLocation, LineTraceEnd,
											FCollisionObjectQueryParams(ECC_PhysicsBody), TraceParams);

	AActor *ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace has hit to %s"), *ActorHit->GetName());
	}
	return Hit;
}

FVector UGrabber::GetLineTraceEnd()
{
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(0, 255, 0),
				  false, 0.f, 0.f, 5.0f);
	return LineTraceEnd;
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckCollision();
	if (PhysicsHandler->GrabbedComponent)
	{
		PhysicsHandler->SetTargetLocation(GetLineTraceEnd());
	}
}