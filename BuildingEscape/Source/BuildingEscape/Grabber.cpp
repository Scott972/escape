// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"




// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();

}

/**Gets Physics Handler*/
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	
	if (PhysicsHandle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("Could Not Locate UPhysicsHandleComponent"));
	}
}

/**Gets Input component and binds actions*/
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent) {
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

/** Intitiates physics handler grab*/
void UGrabber::Grab()
{
	auto InReach = GetFirstPhysicsBodyInReach();

	if (InReach.GetActor()) {
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			InReach.GetComponent(),
			NAME_None,
			InReach.GetComponent()->GetOwner()->GetActorLocation(),
			InReach.GetComponent()->GetOwner()->GetActorRotation()
		);
	}
}

/** Releases any held actor*/
void UGrabber::Release()
{	
	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->ReleaseComponent();
	}

}

/**Send a ray cast out and returns the first object hit*/
const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector LineTraceEnd = GetLineTraceEnd();
	FHitResult LineTraceHit;
	FCollisionQueryParams QueryParams(FName(TEXT("")), false, GetOwner());

	 GetWorld()->LineTraceSingleByObjectType(
		LineTraceHit,
		OUT_PlayerViewPointLocation,
		LineTraceEnd, 
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		QueryParams
	);
	
	 return LineTraceHit; 
}

/** Gets the endpoint of a ray casted from player viewpoint*/
const FVector UGrabber::GetLineTraceEnd()
{

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT_PlayerViewPointLocation, 
		OUT_PlayerViewPointRotation
	);

	FVector LineTraceEnd = OUT_PlayerViewPointLocation + OUT_PlayerViewPointRotation.Vector() * Reach;

	return LineTraceEnd; 
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent) {
		PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
	}
}
