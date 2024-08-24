import json

from settings.constants import Mode

class ExtendedJSONEncoder(json.JSONEncoder):
    def default(self, obj):
        if isinstance(obj, Mode):
            return obj.value
        return super().default(obj)
