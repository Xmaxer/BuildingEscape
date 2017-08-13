// Copyright Kevin Jakubauskas 2017

#pragma once

#include <Engine/TriggerVolume.h>
#include "Engine/SpotLight.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpotLightTrigger.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API USpotLightTrigger : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpotLightTrigger();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
private:

	ASpotLight* SpotLight = nullptr;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;
};
