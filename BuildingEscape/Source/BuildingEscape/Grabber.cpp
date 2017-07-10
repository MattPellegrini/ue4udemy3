// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"

#define OUT


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
    SetupPhysicsHandleComponent();
    SetupInputComponent();
}

void UGrabber::SetupPhysicsHandleComponent()
{
    /// Look for attached physics handle
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    if (!PhysicsHandle) UE_LOG(LogTemp, Error, TEXT("%s has no Physics Handle Component!"), *GetOwner()->GetName());
}

void UGrabber::SetupInputComponent()
{
    /// Look for the Input Component
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if (!InputComponent) UE_LOG(LogTemp, Error, TEXT("%s has no Input Component!"), *GetOwner()->GetName());
    InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
    InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
}


// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
    
    /// If the physics handle is attached, move the object we are holding
    if(PhysicsHandle->GrabbedComponent)
    {
        FVector ReachLineEnd = GetReachLineEnd();
        
        PhysicsHandle->SetTargetLocation(ReachLineEnd);
    }
}

void UGrabber::Grab(){
    UE_LOG(LogTemp, Warning, TEXT("Grab key pressed!"));
    /// Try to reach any actors with physics body collision channel set
    auto HitResult = GetFirstPhysicsBodyInReach();
    auto ComponentToGrab = HitResult.GetComponent();
    auto ActorHit = HitResult.GetActor();
    
    /// If we hit something, attach a physics handle
    if(ActorHit){
        PhysicsHandle->GrabComponent(
            ComponentToGrab,
            NAME_None,
            ComponentToGrab->GetOwner()->GetActorLocation(),
            true //allow rotation.
        );
    }
}

void UGrabber::Release(){
    UE_LOG(LogTemp, Warning, TEXT("Grab key released!"));
    PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
    /// Setup query parameters
    FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
    FVector ReachLineStart = GetReachLineStart();
    FVector ReachLineEnd = GetReachLineEnd();
    
    /// Line-trace (AKA ray-cast) out to reach distance
    FHitResult Hit;
    GetWorld()->LineTraceSingleByObjectType(
                                            OUT Hit,
                                            ReachLineStart,
                                            ReachLineEnd,
                                            FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
                                            TraceParameters
                                            );
    
    /// See what what we hit
    AActor* ActorHit = Hit.GetActor();
    if (ActorHit)
    {
        UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()))
    }

    return Hit;
}

FVector UGrabber::GetReachLineStart() const
{
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
                                                               OUT PlayerViewPointLocation,
                                                               OUT PlayerViewPointRotation
                                                               );
    
    return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd() const
{
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
                                                               OUT PlayerViewPointLocation,
                                                               OUT PlayerViewPointRotation
                                                               );
    
    return (PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach);
}

