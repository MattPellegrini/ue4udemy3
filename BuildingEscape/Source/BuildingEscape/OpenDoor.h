// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenRequest);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

    UPROPERTY(BlueprintAssignable)
    FOnOpenRequest OnOpenRequest;
    
private:
    UPROPERTY(EditAnywhere)
    float OpenAngle = -70.0f;
    
    UPROPERTY(EditAnywhere)
    ATriggerVolume* PressurePlate = nullptr;
    
    UPROPERTY(EditAnywhere)
    float DoorCloseDelaySeconds = 1.0f;
    
    float LastDoorOpenSeconds;
    
    AActor* Owner = nullptr;
    
    void OpenDoor();
    void CloseDoor();
    
    float GetTotalMassOnPressurePlate() const;
    
    float TriggerMass = 35.0f;
};
