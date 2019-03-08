// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"

#define OUT

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

	FindPhysicsHandle();
	SetupInputComponent();
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!physicsHandle) { return; }
	if (physicsHandle->GrabbedComponent)
	{
		physicsHandle->SetTargetLocation(GetReachLineEnd());
	}
	
}

void UGrabber::FindPhysicsHandle()
{
	FString objName = GetOwner()->GetName();
	physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (physicsHandle==nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s Dont have a PhysicsHandle"), *objName);
	}	
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab"));
	auto hitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = hitResult.GetComponent();
	auto actorHit = hitResult.GetActor();
	if (actorHit)
	{
		if (!physicsHandle) { return; }
		physicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
	}
	
}

void UGrabber::Release()
{
	if (!physicsHandle) { return; }
	UE_LOG(LogTemp, Warning, TEXT("Release"));
	physicsHandle->ReleaseComponent();
}


void UGrabber::SetupInputComponent()
{
	FString objName = GetOwner()->GetName();
	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (inputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("input Found"));
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s Dont have a InputComponent"), *objName);
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	/// create raytrace and check hit

	//FVector playerVectorLocation;
	//FRotator playerRotator;

	//GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerVectorLocation, OUT playerRotator);

	//// UE_LOG(LogTemp, Warning, TEXT("PlayerLocation %s PlayerRotation %s"), *playerVectorLocation.ToString(), *playerRotator.ToString());

	//FVector lineTraceEnd = playerVectorLocation + playerRotator.Vector()*reach;

	////DrawDebugLine(GetWorld(), playerVectorLocation, lineTraceEnd, FColor(255, 0, 0), false, 0.f, 0.f, 10.f);

	FHitResult OUT hit;
	FCollisionQueryParams traceParameters(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(hit, GetReachLineStart(), GetReachLineEnd(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), traceParameters);

	AActor* ActorHit = hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("HIT : %s"), *(ActorHit->GetName()));
	}

	return hit;
}

FVector UGrabber::GetReachLineStart()
{

	FVector playerVectorLocation;
	FRotator playerRotator;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerVectorLocation, OUT playerRotator);

	// UE_LOG(LogTemp, Warning, TEXT("PlayerLocation %s PlayerRotation %s"), *playerVectorLocation.ToString(), *playerRotator.ToString());

	return  playerVectorLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	
	FVector playerVectorLocation;
	FRotator playerRotator;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerVectorLocation, OUT playerRotator);

	// UE_LOG(LogTemp, Warning, TEXT("PlayerLocation %s PlayerRotation %s"), *playerVectorLocation.ToString(), *playerRotator.ToString());

	return  playerVectorLocation + playerRotator.Vector()*reach;
}