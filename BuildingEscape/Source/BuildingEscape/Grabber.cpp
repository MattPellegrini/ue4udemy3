// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
    Reach = 100.0f;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
	
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// Get the player viewpoint this tick
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
    
    //Print out the location and rotation
//    UE_LOG(LogTemp,
//           Warning,
//           TEXT("Location: %s \t\t Rotation: %s"),
//           *PlayerViewPointLocation.ToString(),
//           *PlayerViewPointRotation.ToString());

    // Draw a red line for debugging
    FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
    DrawDebugLine(
        GetWorld(),
        PlayerViewPointLocation,
        LineTraceEnd,
        FColor(255,0,0),
        false,
        0.0f,
        0.0f,
        10.0f
    );
    // Ray-cast out to reach distance
    // See what we hit
}

