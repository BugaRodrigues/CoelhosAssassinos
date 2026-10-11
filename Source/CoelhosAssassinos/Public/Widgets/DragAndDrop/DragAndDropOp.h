#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "DragAndDropOp.generated.h"


UCLASS()
class COELHOSASSASSINOS_API UDragAndDropOp : public UDragDropOperation
{
	GENERATED_BODY()

	public:
		UPROPERTY()
		TObjectPtr<UUserWidget> DragedWidget;

};
