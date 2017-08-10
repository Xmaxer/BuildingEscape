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

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	// Look for attached physics handle
	FindPhysicsComponent();
	//Bind key actions to functions
	SetupInputComponent();
}


void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input component found: %s"), *InputComponent->GetReadableName());
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

	if (PhysicsHandle)
	{
		//Physics handler is found
	}
	else
	{
		//Missing physics handle
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName());
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector playerViewPointLoc;
	FRotator playerViewPointRot;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerViewPointLoc, OUT playerViewPointRot);

	FVector lineTraceEnd = playerViewPointLoc + playerViewPointRot.Vector() * Reach;

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(lineTraceEnd);
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	// Get player viewpoint
	FVector playerViewPointLoc;
	FRotator playerViewPointRot;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT playerViewPointLoc, OUT playerViewPointRot);

	FVector lineTraceEnd = playerViewPointLoc + playerViewPointRot.Vector() * Reach;

	///DrawDebugLine(GetWorld(), playerViewPointLoc, lineTraceEnd, FColor(255, 0, 0), false, 0.f, 0, 10.f);

	///UE_LOG(LogTemp, Warning, TEXT("Location %s, Rotation %s"), *playerViewPointLoc.ToString(), *playerViewPointRot.ToString());

	// Ray-cast
	FHitResult LineTraceHit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(OUT LineTraceHit, playerViewPointLoc, lineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);
	// What we hit with Ray-Cast
	AActor* actorHit = LineTraceHit.GetActor();

	if (actorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor hit: %s"), *(actorHit->GetName()));
	}

	return LineTraceHit;
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key pressed."));

	
	///Try and reach actor with physics body collision channel set
	FHitResult hit = GetFirstPhysicsBodyInReach();

	UPrimitiveComponent* ComponentToGrab = hit.GetComponent();

	AActor* ActorHit = hit.GetActor();

	///if we hit an actor, attach physics handle

	if (ActorHit)
	{
		FVector loc = ComponentToGrab->GetOwner()->GetActorLocation();
		FRotator rot = ComponentToGrab->GetOwner()->GetActorRotation();
		PhysicsHandle->GrabComponentAtLocationWithRotation(ComponentToGrab, NAME_None, loc, rot);

	}
	
	
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab key released."));
	/// Release physics handle
	PhysicsHandle->ReleaseComponent();
}

