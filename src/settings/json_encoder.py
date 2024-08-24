import json

class ExtendedJSONEncoder(json.JSONEncoder):
    def default(self, obj):
        return super().default(obj)
