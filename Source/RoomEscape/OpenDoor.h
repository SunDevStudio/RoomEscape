// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine/TriggerVolume.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ROOMESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//void OpenDoor();
	//void CloseDoor();

	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY(BlueprintAssignable)
		FDoorEvent onOpenRequest;

	UPROPERTY(BlueprintAssignable)
		FDoorEvent onCloseRequest;
		
private:
	/*UPROPERTY(EditAnywhere)
		float openAngle = 90.f;*/

	UPROPERTY(EditAnywhere)
		ATriggerVolume* pressurePlate;

	/*UPROPERTY(EditAnywhere)
		float closeDoorDelay = 1.f;*/
	
	AActor* actorThatDoor;

	//float lastTimeCloseDoor;

	AActor *owner;
	float GetTotalMassOfActor();

	UPROPERTY(EditAnywhere)
	float triggerMass = 50.f;
};
