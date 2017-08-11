// Copyright Kevin Jakubauskas 2017

#include "Grabber.h"
#include <GameFramework/PlayerController.h>
#include <Engine/World.h>
#include "DrawDebugHelpers.h"
#include <Components/PrimitiveComponent.h>

#define OUT
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
	FindPhysicsComponent();
	SetupInputComponent();
}


void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing input component component"), *GetOwner()->GetName());
	}
}

void UGrabber::FindPhysicsComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (!PhysicsHandle)
	{
		//Missing physics handle
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName());
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	///DrawDebugLine(GetWorld(), playerViewPointLoc, lineTraceEnd, FColor(255, 0, 0), false, 0.f, 0, 10.f);
	///UE_LOG(LogTemp, Warning, TEXT("Location %s, Rotation %s"), *playerViewPointLoc.ToString(), *playerViewPointRot.ToString());
	// Ray-cast
	FHitResult LineTraceHit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(OUT LineTraceHit, GetLineTraceStart(), GetLineTraceEnd(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);
	// What we hit with Ray-Cast
	AActor* actorHit = LineTraceHit.GetActor();

	if (actorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor hit: %s"), *(actorHit->GetName()));
	}

	return LineTraceHit;
}

FVector UGrabber::GetLineTraceEnd()
{
	FVector playerViewPointLoc;
	FRotator playerViewPointRot;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerViewPointLoc, OUT playerViewPointRot);

	return playerViewPointLoc + playerViewPointRot.Vector() * Reach;
}
FVector UGrabber::GetLineTraceStart()
{
	FVector playerViewPointLoc;
	FRotator playerViewPointRot;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerViewPointLoc, OUT playerViewPointRot);

	return playerViewPointLoc;
}
void UGrabber::Grab()
{
	///Try and reach actor with physics body collision channel set
	FHitResult hit = GetFirstPhysicsBodyInReach();

	UPrimitiveComponent* ComponentToGrab = hit.GetComponent();

	AActor* ActorHit = hit.GetActor();
	if (ActorHit)
	{
		FVector loc = ComponentToGrab->GetOwner()->GetActorLocation();
		FRotator rot = ComponentToGrab->GetOwner()->GetActorRotation();
		PhysicsHandle->GrabComponentAtLocationWithRotation(ComponentToGrab, NAME_None, loc, rot);

	}
}

void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}

