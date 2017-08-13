// Copyright Kevin Jakubauskas 2017

#include "OpenDoor.h"
#include <Components/PrimitiveComponent.h>
#include <GameFramework/Actor.h>
#include <Engine/World.h>
#include <GameFramework/Pawn.h>
#include <GameFramework/PlayerController.h>

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

	Owner = GetOwner();
}


void UOpenDoor::OpenDoor()
{
	if (!Owner) {
		UE_LOG(LogTemp, Error, TEXT("Missing 'Owner' Actor Pointer"));
		return;
	}
	Owner->SetActorRotation(FRotator(0.0f, openDoorAngle, 0.0f), ETeleportType::None);
}
void UOpenDoor::CloseDoor()
{
	if (!Owner) { 
		UE_LOG(LogTemp, Error, TEXT("Missing 'Owner' Actor Pointer"));
		return; }
	Owner->SetActorRotation(FRotator(0.0f, 0.0f, 0.0f), ETeleportType::None);
}
// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Poll trigger volume every frame
	if (GetTotalMassOfActorsOnPlate() > TriggerMass)
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	float CurrentTime = GetWorld()->GetTimeSeconds();

	if ((CurrentTime - LastDoorOpenTime) >= CloseDoorDelay)
	{
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	//Find all overlapping actors
	TArray<AActor*> OverlappingActors;

	if (!PressurePlate) { 
		UE_LOG(LogTemp, Error, TEXT("Missing PressurePlate component value (nullptr)"));
		return 0.0f; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);
	//Iterate through all actors to find masses

	for (const AActor* actor : OverlappingActors)
	{
		TotalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		UE_LOG(LogTemp, Warning, TEXT("%s is on volume"), *actor->GetName())
	}

	return TotalMass;
}
