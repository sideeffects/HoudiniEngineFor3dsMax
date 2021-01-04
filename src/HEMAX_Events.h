#pragma once

class HEMAX_3dsmaxHda;
class HEMAX_UI;

enum class HEMAX_EventType
{
    SessionChanged,
    ShelfUpdated,
    SelectionSetChanged,
    AssetLoaded,
    AssetRemoved,
    HdaPreDelete,
    InputChanged,
    ParameterChanged
};

class HEMAX_Events
{
    public:
                        HEMAX_Events(HEMAX_UI* _UserInterface);
                        HEMAX_Events() = delete;
                        ~HEMAX_Events() = default;

        void            SessionChanged();
        void            ShelfUpdated();
        void            SelectionSetChanged(HEMAX_3dsmaxHda* Hda,
                                            bool ForceUnlock = false);
        void            AssetLoaded();
        void            AssetRemoved();
        void            HdaPreDelete(HEMAX_3dsmaxHda* Hda);

        void            InputChanged();
        void            ParameterChanged();
        void            NodeChanged();

    private:

        HEMAX_UI*       UserInterface;
};
