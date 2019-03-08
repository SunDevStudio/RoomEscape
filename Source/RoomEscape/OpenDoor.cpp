// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

#define OUT
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

	//actorThatDoor = GetWorld()->GetFirstPlayerController()->GetPawn();
	owner = GetOwner();
	
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	// check player trigger to open door
	if (GetTotalMassOfActor()>50.f)
	{
		OpenDoor();
		lastTimeCloseDoor = GetWorld()->GetTimeSeconds();
	}
	float currentTime = GetWorld()->GetTimeSeconds();
	if (currentTime - lastTimeCloseDoor > closeDoorDelay)
	{
		CloseDoor();
	}
	
}

void UOpenDoor::OpenDoor()
{
	owner -> SetActorRotation(FRotator(0.f, -openAngle, 0.f));
}

void UOpenDoor::CloseDoor()
{
	owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}

float UOpenDoor::GetTotalMassOfActor()
{
	float totalMass = 0.f;
	TArray<AActor*> overLappingActor;
	pressurePlate->GetOverlappingActors(OUT overLappingActor);

	for (const auto* actor : overLappingActor)
	{
		totalMass += actor->FindComponentByClass <UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("Name is %s"), *actor->GetName());
	}

	return totalMass;
}