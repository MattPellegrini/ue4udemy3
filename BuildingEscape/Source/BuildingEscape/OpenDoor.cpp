// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"
#include <string>
#define OUT


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
    Owner = GetOwner();
}

void UOpenDoor::OpenDoor()
{
    Owner->SetActorRotation(FRotator(0.0f, OpenAngle, 0.0f));
}

void UOpenDoor::CloseDoor()
{
    Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// poll the trigger volume:
    if(PressurePlate && (GetTotalMassOnPressurePlate() > TriggerMass))
    {
        OpenDoor();
        LastDoorOpenSeconds = GetWorld()->GetTimeSeconds();
    }
    
    if(GetWorld()->GetTimeSeconds() > (LastDoorOpenSeconds + DoorCloseDelaySeconds))
    {
        CloseDoor();
    }
}

float UOpenDoor::GetTotalMassOnPressurePlate() const
{
    float TotalMass = 0.0f;

    if(!PressurePlate){
        return TotalMass;
    }
    
    ///Find all the overlapping actors
    TArray<AActor*> OverlappingActors;
    PressurePlate->GetOverlappingActors(OUT OverlappingActors);
    
    ///Iterate through them and add thier masses
    for(const AActor* Actor: OverlappingActors)
    {
        TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
        UE_LOG(LogTemp, Warning, TEXT("%s kgs on the pressure plate."), *FString::SanitizeFloat(TotalMass));
    }
    
    return TotalMass;
}

