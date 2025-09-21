#include "ACGameModeBase.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "EngineUtils.h" // for TActorIterator

AActor* AACGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<AActor*> Starts;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), Starts);

	for (AActor* Start : Starts)
	{
		// Check if occupied
		bool bOccupied = false;

		for (TActorIterator<APawn> It(GetWorld()); It; ++It)
		{
			if (APawn* OtherPawn = *It)
			{
				if (OtherPawn->GetActorLocation().Equals(Start->GetActorLocation(), 10.f))
				{
					bOccupied = true;
					break;
				}
			}
		}

		if (!bOccupied)
		{
			return Start; // Use the first free spawn
		}
	}

	// If all are "occupied", fall back to engine default
	return Super::ChoosePlayerStart_Implementation(Player);
}
