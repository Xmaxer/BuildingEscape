// Copyright Kevin Jakubauskas 2017

#include "Grabber.h"
#include <GameFramework/PlayerController.h>
#include <Engine/World.h>
#include "DrawDebugHelpers.h"

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

	UE_LOG(LogTemp, Warning, TEXT("Grabber is here!"));
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player viewpoint
	FVector playerViewPointLoc;
	FRotator playerViewPointRot;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerViewPointLoc, OUT playerViewPointRot);

	FVector lineTraceEnd = playerViewPointLoc + playerViewPointRot.Vector() * Reach;

	DrawDebugLine(GetWorld(), playerViewPointLoc, lineTraceEnd, FColor(255, 0, 0), false, 0.f, 0, 10.f);
	//UE_LOG(LogTemp, Warning, TEXT("Location %s, Rotation %s"), *playerViewPointLoc.ToString(), *playerViewPointRot.ToString());
	// Ray-cast

	// What we hit with Ray-Cast
}

