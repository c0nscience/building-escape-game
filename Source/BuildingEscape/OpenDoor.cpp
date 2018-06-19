// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0.f, OpenDoorAngle, 0.f));
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}



// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float CurrentTime = GetWorld()->GetTimeSeconds();

	if (GetTotalMassOfActorsOnPlate() > 50.f)
	{
		OpenDoor();
		LastDoorOpenTime = CurrentTime;
	}

	if (CurrentTime - LastDoorOpenTime > DoorCloseDelay) {
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.f;

	TArray<AActor*> OverlappingActors;

	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (auto* Actor : OverlappingActors)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor on Plate: %s"), *Actor->GetName())

		auto PrimitiveComponent = Actor->FindComponentByClass<UPrimitiveComponent>();
		
		if (PrimitiveComponent) {
			TotalMass += PrimitiveComponent->GetMass();
		}
	}

	return TotalMass;
}

