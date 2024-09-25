// Copyright Epic Games, Inc. All Rights Reserved.

#include "DonkeyKong_L02GameMode.h"
#include "DonkeyKong_L02Character.h"
#include "UObject/ConstructorHelpers.h"
#include "ObstaculoMuro.h"
#include "Capsula.h"
#include "componentePlataforma.h"
#include "Barril.h"
#include "Esfera.h"

ADonkeyKong_L02GameMode::ADonkeyKong_L02GameMode()
{
	// establecer la clase de jugador predeterminada
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/SideScrollerCPP/Blueprints/SideScrollerCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ADonkeyKong_L02GameMode::BeginPlay()
{
    Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Yellow, TEXT("Plataformas creadas..."));


	// Crear un obstaculo en la parte superior de la escena
	FTransform SpawnLocation;
	SpawnLocation.SetLocation(FVector(1160.0f, -1210.0f, 340.0f));
	SpawnLocation.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
	obstaculo01 = GetWorld()->SpawnActor<AObstaculoMuro>(AObstaculoMuro::StaticClass(), SpawnLocation);
	
	// Valores iniciales para la creacion de las plataformas
	FVector posicionInicial = FVector(1160.0f, -1100.0f, 400.f);
	FRotator rotacionInicial = FRotator(0.0f, 0.0f, 13.0f);
	FTransform SpawnLocationCP;
	float incrementoAltura = 500.0f; // Incremento de altura entre plataformas
	float anchoComponentePlataforma = 350.0f;
	float incrementoAltoComponentePlataforma = -105.0f;
	float incrementoAltoEntrePisos = 1500.0f;
	float incrementoInicioPiso = 100.0f;


	for (int npp = 0; npp < 10; npp++)
	{
		rotacionInicial.Roll = rotacionInicial.Roll * -1;
		incrementoInicioPiso = incrementoInicioPiso * -1;
		incrementoAltoComponentePlataforma = incrementoAltoComponentePlataforma * -1;
		SpawnLocationCP.SetRotation(FQuat(rotacionInicial));

		for (int ncp = 0; ncp < 10; ncp++) {
			SpawnLocationCP.SetLocation(FVector(posicionInicial.X, posicionInicial.Y + anchoComponentePlataforma * ncp, posicionInicial.Z));
			if (npp % 2 != 0) {
				AcomponentePlataforma* cp = GetWorld()->SpawnActor<AcomponentePlataforma>(AcomponentePlataforma::StaticClass(), SpawnLocationCP);
					
				if (ncp == 1) {
					cp->bMoverHorizontalmente = true;
					cp->StartPoint = FVector(posicionInicial.X, posicionInicial.Y, posicionInicial.Z);
					cp->EndPoint = FVector(posicionInicial.X + 1000.0f, posicionInicial.Y, posicionInicial.Z);
			}
			componentesPlataforma.Add(cp);
			//componentesPlataforma.Add(GetWorld()->SpawnActor<AcomponentePlataforma>(AcomponentePlataforma::StaticClass(), SpawnLocationCP));

			if (ncp < 9) {
					posicionInicial.Z = posicionInicial.Z + incrementoAltoComponentePlataforma;
						}
			}
			else {
				componentesPlataforma.Add(GetWorld()->SpawnActor<AcomponentePlataforma>(AcomponentePlataforma::StaticClass(), SpawnLocationCP));
				if (ncp < 9) {
					posicionInicial.Z = posicionInicial.Z + incrementoAltoComponentePlataforma;
				}
			}
		}

		posicionInicial.Z = posicionInicial.Z + incrementoAltoEntrePisos;
		posicionInicial.Y = posicionInicial.Y + incrementoInicioPiso;
	}


	GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Green, TEXT("Plataformas verticales creadas..."));
	

	// Crear plataformas verticales
	// valores iniciales para la creacion de las platafoformas verticales
	FVector posicionInicialPlataforma = FVector(1160.0f, -1700.0f, 400.f);
	FRotator rotacionInicialPlataforma = FRotator(0.0f, 0.0f, 0.0f);
	float movimientoVerticalPlataforma = 500.0f;

	for (int i = 0; i < 5; i++)
	{
		FTransform SpawnLocationPlataforma;
		SpawnLocationPlataforma.SetLocation(posicionInicialPlataforma);
		AcomponentePlataforma* Plataforma = GetWorld()->SpawnActor<AcomponentePlataforma>(AcomponentePlataforma::StaticClass(), SpawnLocationPlataforma);

		// Activamos el movimiento solo vertical para cada plataforma
		Plataforma->bMoverSoloVertical = true; // Este flag activa el movimiento vertical solo para estas plataformas
		Plataforma->StartPoint = posicionInicialPlataforma; // Posición inicial
		Plataforma->EndPoint = FVector(posicionInicialPlataforma.X, posicionInicialPlataforma.Y, posicionInicialPlataforma.Z + 2300.0f); // Posición final (movimiento vertical)

		componentesPlataforma.Add(Plataforma);

		// Incrementa la altura para la siguiente plataforma
		if (i < 2) {
			// Para las primeras dos plataformas
			posicionInicialPlataforma.Y += 4400; // Mantener la misma distancia en Y
			posicionInicialPlataforma.Z += movimientoVerticalPlataforma;
		}
		else {
			// Para las últimas dos plataformas, aumentar más la Z
			posicionInicialPlataforma.Y -= 4400; // Mantener la misma distancia en Y
			posicionInicialPlataforma.Z += movimientoVerticalPlataforma + 4000.0f; // Aumentar Z en más de 1000
		}
	}



	GetWorld()->GetTimerManager().SetTimer(SpawnBarrilTimerHandle, this, &ADonkeyKong_L02GameMode::SpawnBarril, 3.0f, true);
	
	//Crear una esfera en la parte superior del primer piso que baje rebotando hasta llegar al suelo
	
	/*for (int nes = 0; nes < 5; nes++) {
		FTransform SpawnLocationEsfera;
		SpawnLocationEsfera.SetLocation(FVector(1300.0f, -200.0f, 1060.0f));
		SpawnLocationEsfera.SetRotation(FQuat(FRotator(0.0f, 0.0f, 0.0f)));
		esferasMap.Add(nes, GetWorld()->SpawnActor<AEsfera>(AEsfera::StaticClass(), SpawnLocationEsfera));
	}*/
}

void ADonkeyKong_L02GameMode::SpawnBarril()
{
	//if (barriles.Num() >= numeroMaximoBarriles) {
	if (contadorBarriles < numeroMaximoBarriles) {
		//Spawn de un objeto barril en la escena sobre la primera plataforma
		FTransform SpawnLocationBarril;
		SpawnLocationBarril.SetLocation(FVector(1160.0f, 900.0f, 4120.0f));
		SpawnLocationBarril.SetRotation(FQuat(FRotator(90.0f, 0.0f, 0.0f)));
		//ABarril* barril01 = GetWorld()->SpawnActor<ABarril>(ABarril::StaticClass(), SpawnLocationBarril);
		barriles.Add(GetWorld()->SpawnActor<ABarril>(ABarril::StaticClass(), SpawnLocationBarril));
		contadorBarriles++;

		if (contadorBarriles >= numeroMaximoBarriles) {
			GetWorld()->GetTimerManager().ClearTimer(SpawnBarrilTimerHandle);
		}
	}
}
