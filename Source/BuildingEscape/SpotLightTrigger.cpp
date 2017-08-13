// Copyright Kevin Jakubauskas 2017

#include "SpotLightTrigger.h"
#include "Components/SpotLightComponent.h"
#include <Components/PrimitiveComponent.h>
#include <GameFramework/Actor.h>

#define OUT
// Sets default values for this component's properties
USpotLightTrigger::USpotLightTrigger()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpotLightTrigger::BeginPlay()
{
	Super::BeginPlay();

	SpotLight = (ASpotLight*) GetOwner();
}


// Called every frame
void USpotLightTrigger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate)
	{
		TArray<AActor*> OverlappingActors;

		PressurePlate->GetOverlappingActors(OUT OverlappingActors);

		float TotalMass = 0.f;

		for (const AActor* actor : OverlappingActors)
		{
			TotalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		}
		if (SpotLight)
		{
			if (TotalMass >= 30.f)
			{
				SpotLight->SetLightColor(FLinearColor(0.f, 255.f, 0.f));

			}
			else
			{
				SpotLight->SetLightColor(FLinearColor(255.f, 0.f, 0.f));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Couldn't get spotlight!"))
		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't get pressure plate!"))
	}
}

