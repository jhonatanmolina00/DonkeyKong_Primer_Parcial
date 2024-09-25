// Fill out your copyright notice in the Description page of Project Settings.


#include "componentePlataforma.h"

// Sets default values
AcomponentePlataforma::AcomponentePlataforma()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Game/Geometry/Meshes/ComponentePlataforoma.ComponentePlataforoma'"));

	meshPlataforma = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	meshPlataforma->SetStaticMesh(MeshAsset.Object);
	RootComponent = meshPlataforma;

	FVector nuevaScalaPlataforma(3.0f, 4.2f, 1.0f); // Cambia estos valores según tus necesidades
	SetActorScale3D(nuevaScalaPlataforma);
	bMoverHorizontalmente = false;
	bMoverVerticalmente = false;
	bMoverSoloVertical = false;

	StartPoint = FVector(0.0f, 0.0f, 0.0f);
	EndPoint = FVector(1000.0f, 0.0f, 0.0f);
	Speed = 500.0f;
}

// Called when the game starts or when spawned
void AcomponentePlataforma::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AcomponentePlataforma::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bMoverVerticalmente || bMoverSoloVertical)
	{
		FVector CurrentLocation = GetActorLocation();
		FVector TargetLocation = (FVector::Dist(CurrentLocation, StartPoint) < 10.0f) ? EndPoint : StartPoint;
		FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
		FVector NewLocation = CurrentLocation + Direction * Speed * DeltaTime;

		SetActorLocation(NewLocation);

		if (FVector::Dist(NewLocation, TargetLocation) < 10.0f)
		{
			// Invertir la dirección para un movimiento continuo
			FVector Temp = StartPoint;
			StartPoint = EndPoint;
			EndPoint = Temp;
		}
	}

}

