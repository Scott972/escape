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
	GetPhysicsComponent();
	GetInputComponent();

}

/**Gets Physics Handler*/
void UGrabber::GetPhysicsComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

}

/**Gets Input component and binds actions*/
void UGrabber::GetInputComponent()
{
	Input = GetOwner()->FindComponentByClass<UInputComponent>();

	if (Input) {
		Input->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		Input->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("Boy this course is shit"));
	}
}


void UGrabber::Grab()
{
	GetFirstPhysicsBodyInReach();
}


void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("We not grabbing :("));

}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector OUT_PlayerViewPointLocation;
	FRotator OUT_PlayerViewPointRotation;

	FVector LineTraceEnd = (OUT_PlayerViewPointRotation.Vector() * Reach) + OUT_PlayerViewPointLocation;
	/*Careful, the below statment produces out values*/
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT_PlayerViewPointLocation, OUT_PlayerViewPointRotation);
	FHitResult LineTraceHit;

	FCollisionQueryParams QueryParams(FName(TEXT("")), false, GetOwner());

	//ray cast out to reach distance (to what???)
	bool bHasHitActor = GetWorld()->LineTraceSingleByObjectType(
		LineTraceHit, OUT_PlayerViewPointLocation,
		LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		QueryParams
	);

	if (bHasHitActor) {
		FString ActorHit = LineTraceHit.GetActor()->GetName();
		UE_LOG(LogTemp, Warning, TEXT("Colliding with %s"), *ActorHit);

	}

	return FHitResult();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//if physics attached
		//move object with us
}

